class AlterUserTableGender < ActiveRecord::Migration
  def self.up
    add_column :users, :gender, :string, :limit => 6
  end

  def self.down
    remove_column :users, :gender
  end
end
