class AlterUserForActivation < ActiveRecord::Migration
  def self.up
    add_column :users, :activation_code, :string, :limit => 40
    add_column :users, :activated_at, :datetime
    add_column :users, :state, :string, :null => :no, :default => 'passive'
    add_column :users, :deleted_at, :datetime
  end

  def self.down
    remove_column :users, :activation_code
    remove_column :users, :activated_at
    remove_column :users, :state
    remove_column :users, :delete_at
  end
end
