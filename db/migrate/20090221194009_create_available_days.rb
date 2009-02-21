class CreateAvailableDays < ActiveRecord::Migration
  def self.up
    create_table :available_days do |t|
      t.column :day,  :string,  :limit => 50

      t.timestamps
    end
  end

  def self.down
    drop_table :available_days
  end
end
