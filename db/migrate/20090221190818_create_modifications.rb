class CreateModifications < ActiveRecord::Migration
  def self.up
    create_table :modifications do |t|
      t.column  :transaction_id,  :integer, :null => false
      t.column  :approved,        :boolean, :default => 0
      t.column  :created_at,      :timestamp
      t.column  :check_in,        :date
      t.column  :check_out,       :date
      t.column  :guests,          :integer
      t.column  :comments,        :string

      t.timestamps
    end
  end

  def self.down
    drop_table :modifications
  end
end
